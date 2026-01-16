import { marked, type TokenizerExtension, type RendererExtension } from 'marked';

// Callout types with their icons and colors
const calloutTypes: Record<string, { icon: string; colorClass: string; title: string }> = {
  info: {
    icon: '💡',
    colorClass: 'callout-info',
    title: 'Information'
  },
  tip: {
    icon: '✨',
    colorClass: 'callout-tip',
    title: 'Astuce'
  },
  warning: {
    icon: '⚠️',
    colorClass: 'callout-warning',
    title: 'Attention'
  },
  remember: {
    icon: '🧠',
    colorClass: 'callout-remember',
    title: 'A retenir'
  },
  example: {
    icon: '📝',
    colorClass: 'callout-example',
    title: 'Exemple'
  }
};

interface CalloutToken {
  type: 'callout';
  raw: string;
  calloutType: string;
  content: string;
}

// Custom extension for callout blocks
const calloutExtension: TokenizerExtension & RendererExtension = {
  name: 'callout',
  level: 'block',
  start(src: string) {
    return src.match(/^:::/)?.index;
  },
  tokenizer(src: string): CalloutToken | undefined {
    const rule = /^:::(\w+)\n([\s\S]*?)\n:::/;
    const match = rule.exec(src);
    if (match) {
      return {
        type: 'callout',
        raw: match[0],
        calloutType: match[1],
        content: match[2].trim()
      };
    }
    return undefined;
  },
  renderer(token: CalloutToken): string {
    const config = calloutTypes[token.calloutType] || calloutTypes.info;
    const innerHtml = marked.parse(token.content) as string;
    return `
      <div class="callout ${config.colorClass}">
        <div class="callout-header">
          <span class="callout-icon">${config.icon}</span>
          <span class="callout-title">${config.title}</span>
        </div>
        <div class="callout-content">
          ${innerHtml}
        </div>
      </div>
    `;
  }
};

// Configure marked with the extension
marked.use({ extensions: [calloutExtension] });

export function parseMarkdown(content: string): string {
  return marked(content) as string;
}
